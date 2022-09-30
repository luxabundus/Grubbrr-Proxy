class Rowset {
	rows = [];
	columns = [];
	columnIndexes = {};

	constructor(json) {
		if (json) {
			if (json.rows) {
				this.rows = json.rows;
			}

			if (json.columns) {
				this.columns = json.columns;
				for (let i = 0; i < this.columns.length; i++) {
					this.columnIndexes[this.columns[i]] = i;
				}
			}
		}
	}

	get length() {
		return this.rows.length;
	}

	getJson() {
		return {
			columns: this.columns,
			rows: this.rows
		}
	}

	*getRows() {
		const columnIndexes = this.columnIndexes;

		for (const row of this.rows) {
			yield {	// e.g. row.value("name")
				value(columnName) {
					return row[columnIndexes[columnName]];
				}
			};
		}
	}

	pushRow(object) {
		let row = [];
		for (const key in object) {
			row[this.columnIndexes[key]] = object[key];
		}
		this.rows.push(row);
	}
}


(function () {
	let contentList = $("#contentList");

	const contentItemTemplate = contentList.find(".contentItem");
	contentItemTemplate.remove();

	// Define list insertion
	function insertContentItems(json) {
		const contentRowset = new Rowset(json);

		for (const row of contentRowset.getRows()) {
			let itemIsFile = row.value("isFile");
			let itemName = row.value("name");
			let itemId = itemIsFile + "-" + itemName.replace(/[#:\. ]/g, "-");

			let contentItem = contentList.find("#" + itemId); // key on item id

			if (!contentItem.length) { // item doesn't exist; add now
				contentItem = contentItemTemplate.clone(true);
				contentItem.attr("id", itemId);
				contentItem.show();

				let contentLink = contentItem.find("a");

				if (itemIsFile) {
					contentLink.text(itemName);
					contentLink.attr("href", itemName + "?download");
					contentLink.attr("download", itemName);
				}
				else { // item is a directory
					contentLink.text("[" + itemName + "]");

					if (itemName === "..") { // link to parent directory
						let pathname = window.location.pathname;
						let lastPos = pathname.lastIndexOf("/", pathname.length - 2);
						contentLink.attr("href", pathname.substr(0, lastPos + 1));
					}
					else { // link to child directory
						contentLink.attr("href", itemName + "/");
					}

					// For now, disable delete for directories.
					contentItem.find("input:checkbox").css("visibility", "hidden");
				}

				contentList.append(contentItem);
			}
		}

		sortContentItems();
	}

	// Define list sorting
	function sortContentItems() {
		let children = contentList.children("div");

		children.sort(
			function (left, right) {
				return left.id.localeCompare(right.id, undefined, { sensitivity: 'accent' });
			}
		);

		contentList.append(children);
	}

	// Define list selection query
	function querySelectedContents() {
		let contents = new Rowset({ columns: ["id", "name"] });

		contentList.find("input:checked").each(
			function () {
				const contentItem = $(this).parent();
				const itemLink = contentItem.find("a");
				contents.pushRow({ id: contentItem.attr("id"), name: itemLink.text() });
			});

		return contents;
	}

	// Define list deletion
	$("#deleteButton")
		.on("click",
			function () {
				const selection = querySelectedContents();

				if (selection.length) {
					const request = {
						contents: selection.getJson()
					};

					$.ajax({
						method: "DELETE",
						url: "?contents",
						data: JSON.stringify(request),
						contentType: false,
						processData: false,
						success: function (response) {
							const contentRowset = new Rowset(response.contents);

							for (const row of contentRowset.getRows()) {
								if (!row.value("error")) {
									const id = row.value("id");
									$("#" + row.value("id")).remove();
								}
							}
						},
						error: function () {
							alert("Delete Error!");
						}
					});
				}
			}
		);

	// Define upload functionality
	function uploadFiles(formData) {
		$.ajax({
			method: "PUT",
			url: "?contents",
			data: formData,
			contentType: false,
			processData: false,
			success: function (response) {
				insertContentItems(response.contents);
			},
			error: function () {
				alert("Upload Error!");
			}
		});
	}

	$("#uploadButton")
		.on("click",
			function () {
				$("#uploadInput").click();
			}
		);

	$("#uploadInput")
		.on("change",
			function () {
				const files = $(this)[0].files;

				if (files.length) {
					let formData = new FormData();

					for (const file of files) {
						formData.append('file', file);
					}

					uploadFiles(formData);
				}
			}
		);

	// Define drag-drop handling
	contentList
		.on("dragover",
			function (ev) {
				ev.preventDefault();
				ev.stopPropagation();
			}
		)
		.on("drop",
			function (ev) {
				ev.preventDefault();
				ev.stopPropagation();

				const dataTransfer = ev.originalEvent.dataTransfer;

				if (dataTransfer) {
					let formData = new FormData();

					if (dataTransfer.items) {
						for (const item of dataTransfer.items) {
							if (item.kind === "file") {
								formData.append("file", item.getAsFile());
							}
						}
					}
					else {
						for (const file of dataTransfer.files) {
							formData.append("file", file);
						}
					}

					uploadFiles(formData);
				}
			}
		);

	// Do initial content query
	$.ajax({
		method: "GET",
		url: "?contents",
		success: function (response) {
			insertContentItems(response.contents);
		},
		error: function () {
			alert("Query Error!");
		}
	});
}());